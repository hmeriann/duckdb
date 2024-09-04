SELECT 
  list_aggregate(tau_pt, 'sem'),
  list_aggregate(tau_eta, 'sem'),
  list_aggregate(jet_pt, 'sem'),
  list_aggregate(jet_eta, 'sem'),
  list_aggregate(muon_pt, 'sem'),
  list_aggregate(muon_eta, 'sem'),
  list_aggregate(el_pt, 'sem'),
  list_aggregate(el_eta, 'sem'),
  list_aggregate(ph_pt, 'sem'),
  list_aggregate(ph_eta, 'sem')  
FROM singleMu;