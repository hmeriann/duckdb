SELECT 
  list_aggregate(tau_pt, 'min'),
  list_aggregate(tau_eta, 'min'),
  list_aggregate(jet_pt, 'min'),
  list_aggregate(jet_eta, 'min'),
  list_aggregate(muon_pt, 'min'),
  list_aggregate(muon_eta, 'min'),
  list_aggregate(el_pt, 'min'),
  list_aggregate(el_eta, 'min'),
  list_aggregate(ph_pt, 'min'),
  list_aggregate(ph_eta, 'min')
FROM singleMu;