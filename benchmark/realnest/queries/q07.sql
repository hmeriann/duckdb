SELECT 
  list_aggregate(tau_pt, 'count'),
  list_aggregate(tau_eta, 'count'),
  list_aggregate(jet_pt, 'count'),
  list_aggregate(jet_eta, 'count'),
  list_aggregate(muon_pt, 'count'),
  list_aggregate(muon_eta, 'count'),
  list_aggregate(el_pt, 'count'),
  list_aggregate(el_eta, 'count'),
  list_aggregate(ph_pt, 'count'),
  list_aggregate(ph_eta, 'count')  
FROM singleMu;